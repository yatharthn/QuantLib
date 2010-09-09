/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2010 Ferdinando Ametrano

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file btp.hpp
    \brief Italian BTP (Buoni Poliennali del Tesoro) fixed rate bond
*/

#ifndef quantlib_btp_hpp
#define quantlib_btp_hpp

#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/instruments/vanillaswap.hpp>

#include <numeric>

namespace QuantLib {

    //! Italian BTP (Buoni Poliennali del Tesoro) fixed rate bond
    /*! \ingroup instruments

    */
    class BTP : public FixedRateBond {
      public:
        BTP(const Date& maturityDate,
            Rate fixedRate,
            Real redemption = 100.0,
            const Date& startDate = Date(),
            const Date& issueDate = Date());
        //! BTP yield given a (clean) price and settlement date
        /*! The default BTP conventions are used: Actual/Actual (ISMA),
            Compounded, Annual.
            The default bond settlement is used if no date is given. */
        Rate yield(Real cleanPrice,
                   Date settlementDate = Date(),
                   Real accuracy = 1.0e-8,
                   Size maxEvaluations = 100) const;
    };

    class RendistatoBasket : public Observer,
                             public Observable {
      public:
        RendistatoBasket(const std::vector<boost::shared_ptr<BTP> >& btps,
                         const std::vector<Real>& outstandings,
                         const std::vector<Handle<Quote> >& cleanPriceQuotes);
        //! \name Inspectors
        //@{
        Size size() const { return n_;}
        const std::vector<boost::shared_ptr<BTP> >& btps() const { return btps_;}
        const std::vector<Real>& outstandings() const { return outstandings_;}
        const std::vector<Handle<Quote> >& cleanPriceQuotes() const { return quotes_;}
        const std::vector<Real>& weights() const { return weights_;}
        Real outstanding() const { return outstanding_;}
        //@}
        //! \name Observer interface
        //@{
        void update() { notifyObservers(); }
        //@}
      private:
        std::vector<boost::shared_ptr<BTP> > btps_;
        std::vector<Real> outstandings_;
        std::vector<Handle<Quote> > quotes_;
        Real outstanding_;
        Size n_;
        std::vector<Real> weights_;
    };

    class RendistatoCalculator : LazyObject {
      public:
        RendistatoCalculator(const boost::shared_ptr<RendistatoBasket>& basket,
                             const boost::shared_ptr<Euribor>& euriborIndex,
                             const Handle<YieldTermStructure>& discountCurve);
        //! \name Calculations
        //@{
        Rate yield() const;
        const std::vector<Rate>& yields() const;
        Time duration() const;
        const std::vector<Time>& durations() const;
        //@}
        //! \name Equivalent Swap proxy
        //@{
        boost::shared_ptr<VanillaSwap> equivalentSwap() const;
        Rate equivalentSwapRate() const { return equivalentSwap()->fairRate();}
        Time equivalentSwapLength() const;
        Spread equivalentSwapSpread() const;
        //@}
      protected:
        //! \name LazyObject interface
        //@{
        void performCalculations() const;
        //@}
      private:
        boost::shared_ptr<RendistatoBasket> basket_;
        boost::shared_ptr<Euribor> euriborIndex_;
        Handle<YieldTermStructure> discountCurve_;

        mutable std::vector<Rate> yields_;
        mutable std::vector<Time> durations_;
        mutable Time duration_;
        mutable Size equivalentSwapIndex_;

        std::vector<boost::shared_ptr<VanillaSwap> > swaps_;
        std::vector<Time> swapLenghts_;
        Size nSwaps_;
    };

    //! RendistatoCalculator equivalent swap lenth Quote adapter
    class RendistatoEquivalentSwapLengthQuote : public Quote {
      public:
        RendistatoEquivalentSwapLengthQuote(
            const boost::shared_ptr<RendistatoCalculator>& r);
        Real value() const;
        bool isValid() const;
      private:
        boost::shared_ptr<RendistatoCalculator> r_;
    };

    //! RendistatoCalculator equivalent swap spread Quote adapter
    class RendistatoEquivalentSwapSpreadQuote : public Quote {
      public:
        RendistatoEquivalentSwapSpreadQuote(
            const boost::shared_ptr<RendistatoCalculator>& r);
        Real value() const;
        bool isValid() const;
      private:
        boost::shared_ptr<RendistatoCalculator> r_;
    };

    // inline
    
    inline Rate RendistatoCalculator::yield() const {
        return std::inner_product(basket_->weights().begin(),
                                  basket_->weights().end(),
                                  yields().begin(), 0.0);
    }

    inline const std::vector<Rate>& RendistatoCalculator::yields() const {
        calculate();
        return yields_;
    }

    inline Time RendistatoCalculator::duration() const {
        calculate();
        return duration_;
    }

    inline const std::vector<Time>& RendistatoCalculator::durations() const {
        calculate();
        return durations_;
    }

    inline boost::shared_ptr<VanillaSwap>
    RendistatoCalculator::equivalentSwap() const {
        calculate();
        return swaps_[equivalentSwapIndex_];
    }

    inline Time RendistatoCalculator::equivalentSwapLength() const {
        calculate();
        return swapLenghts_[equivalentSwapIndex_];
    }

    inline Spread RendistatoCalculator::equivalentSwapSpread() const {
        return yield() - equivalentSwapRate();
    }

    inline Real RendistatoEquivalentSwapLengthQuote::value() const {
        return r_->equivalentSwapLength();
    }

    inline Real RendistatoEquivalentSwapSpreadQuote::value() const {
        return r_->equivalentSwapSpread();
    }

}

#endif